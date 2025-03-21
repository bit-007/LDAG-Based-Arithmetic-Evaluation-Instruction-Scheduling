Here’s a simpler and more concise `README.md` file:

```markdown
# LDAG Symbolic Execution and Scheduling

This program demonstrates symbolic execution and instruction scheduling on a Linear Directed Acyclic Graph (LDAG). It calculates node levels, QH positions, and distances between nodes, and performs instruction scheduling using in-order traversal.

## How It Works
1. **LDAG Construction**: The program builds an LDAG representing an arithmetic expression.
2. **Level Calculation**: Assigns levels to each node based on its position in the graph.
3. **Symbolic Execution**: Computes QH positions for each node.
4. **Distance Calculation**: Uses BFS to calculate the distance between two nodes.
5. **Instruction Scheduling**: Performs in-order traversal to schedule instructions and prints the queue.

## Usage
1. Compile the program:
   ```bash
   gcc -o ldag_program ldag_program.c
   ```
2. Run the executable:
   ```bash
   ./ldag_program
   ```

## Output
The program prints:
- Node levels and QH positions.
- Distance between two specified nodes.
- Instruction scheduling queue.

## Dependencies
- C compiler (e.g., `gcc`).

